open Models;
open Utils;

let makeModels = data => {
  let notecrafts =
    data.observations
    |> List.filter(o => List.hd(o.code.coding).code == "gen")
    |> List.map(o =>
         {
           notecraftId: o.observationId,
           category: List.hd(List.hd(o.category).coding).code,
           ncpid: extractId(o.subject.ref),
           value: o.value,
         }
       );
  let exams =
    data.observations
    |> List.filter(o => List.hd(o.code.coding).code == "29545-1")
    |> List.map(o =>
         {
           examId: o.observationId,
           epid: extractId(o.subject.ref),
           value: o.value,
         }
       );
  let comments =
    data.clinicalImpressions
    |> List.filter(ci => ci.description == "Comment")
    |> List.map(ci =>
         {
           commentId: ci.clinicalImpressionId,
           cpid: extractId(ci.subject.ref),
           effectiveDateTime: ci.dateTime,
           assessor: ci.assessor.ref,
           summary: ci.summary,
         }
       );
  let notes =
    data.clinicalImpressions
    |> List.filter(ci => ci.description == "Note")
    |> List.map(ci =>
         {
           noteId: ci.clinicalImpressionId,
           npid: extractId(ci.subject.ref),
           date: ci.dateTime,
           text: ci.summary,
         }
       );
  {
    patients: data.patients,
    comments,
    notes,
    practitioners: data.practitioners,
    teams: data.teams,
    notecrafts,
    exams,
  };
};

let getTeamPatients = (~patients, ~currentTeam) =>
  currentTeam.items
  |> List.map(i => extractId(i.item.ref))
  |> List.map(id => List.filter(p => p.pid == id, patients))
  |> List.flatten;

let getCurrentPatient = (~teamPatients) =>
  List.length(teamPatients) == 0 ? None : Some(teamPatients |> List.hd);

let getPatientComments = (~comments, ~currentPatient) =>
  switch (currentPatient) {
  | None => []
  | Some(currentPatient) =>
    comments |> List.filter(c => c.cpid == currentPatient.pid)
  };

let getPatientNotes = (~notes, ~currentPatient) =>
  switch (currentPatient) {
  | None => []
  | Some(currentPatient) =>
    notes |> List.filter(n => n.npid == currentPatient.pid)
  };

let getCurrentNote = (~patientNotes) =>
  List.length(patientNotes) == 0 ? None : Some(patientNotes |> List.hd);

let getPatientNotecrafts = (~notecrafts, ~currentPatient) =>
  switch (currentPatient) {
  | None => []
  | Some(currentPatient) =>
    notecrafts |> List.filter(n => n.ncpid == currentPatient.pid)
  };

let getPatientExams = (~exams: list(exam), ~currentPatient) =>
  switch (currentPatient) {
  | None => []
  | Some(currentPatient) =>
    exams |> List.filter(e => e.epid == currentPatient.pid)
  };

let models: models = Decoder.data |> makeModels;

type state = {
  practitioners: list(practitioner),
  currentUser: string,
  notecrafts: list(notecraft),
  exams: list(exam),
  teams: list(team),
  patients: list(patient),
  comments: list(comment),
  notes: list(note),
  currentTeam: team,
  currentPatient: option(patient),
  currentNote: option(note),
};

type action =
  | SwitchPatient(patient)
  | RemovePatient(patient)
  | AddPatient(patient)
  | AddComment(comment)
  | SwitchNote(note)
  | AddNote(note)
  | SwitchTeam(team);

let component = ReasonReact.reducerComponent("NoteAssembler");

let make = (~practitionerId="Dr. Foo", ~storage, _children) => {
  ...component,
  initialState: () => {
    let subscribedLists =
      (models.teams |> List.filter(g => g.title == practitionerId) |> List.hd).
        items
      |> List.map(r => extractId(r.item.ref))
      |> List.map(id => List.filter(l => l.teamId == id, models.teams))
      |> List.flatten;

    let patients =
      subscribedLists
      |> List.map(l => l.items)
      |> List.flatten
      |> List.map(p => extractId(p.item.ref))
      |> List.map(id => List.filter(p => p.pid == id, models.patients))
      |> List.flatten;

    let comments =
      patients
      |> List.map(p => p.pid)
      |> List.map(p => List.filter(c => c.cpid == p, models.comments))
      |> List.flatten;

    let notes =
      patients
      |> List.map(p => p.pid)
      |> List.map(p => List.filter(n => n.npid == p, models.notes))
      |> List.flatten;

    let currentTeam = subscribedLists |> List.hd;
    let currentPatient =
      getCurrentPatient(getTeamPatients(patients, currentTeam));
    let currentNote = getCurrentNote(getPatientNotes(notes, currentPatient));

    {
      practitioners: models.practitioners,
      currentUser: practitionerId,
      teams: subscribedLists,
      patients,
      comments,
      notes,
      notecrafts: models.notecrafts,
      exams: models.exams,
      currentTeam,
      currentPatient,
      currentNote,
    };
  },
  reducer: (action, state) =>
    switch (action) {
    | SwitchPatient(newPatient) =>
      let patientNotes = getPatientNotes(state.notes, Some(newPatient));
      let newNote = getCurrentNote(patientNotes);
      ReasonReact.Update({
        ...state,
        currentPatient: Some(newPatient),
        currentNote: newNote,
      });
    | RemovePatient(oldPatient) =>
      let newPatients =
        List.filter(p => p.pid != oldPatient.pid, state.patients);
      let newPatient = ref(state.currentPatient);
      if (oldPatient == Js.Option.getExn(newPatient^)) {
        let teamPatients = getTeamPatients(newPatients, state.currentTeam);
        newPatient := getCurrentPatient(teamPatients);
      };
      ReasonReact.Update({
        ...state,
        patients: newPatients,
        currentPatient: newPatient^,
      });
    | AddPatient(newPatient) =>
      let newPatients = List.append(state.patients, [newPatient]);
      let newEntry = {
        item: {
          ref: "Patient/" ++ newPatient.pid,
        },
      };
      let newEntries = List.append(state.currentTeam.items, [newEntry]);
      let newTeam = {...state.currentTeam, items: newEntries};
      let newTeams =
        List.append(
          List.filter(team => team != state.currentTeam, state.teams),
          [newTeam],
        );
      ReasonReact.Update({
        ...state,
        patients: newPatients,
        teams: newTeams,
        currentTeam: newTeam,
      });
    | AddComment(newComment) =>
      ReasonReact.Update({
        ...state,
        comments: List.append(state.comments, [newComment]),
      })
    | SwitchNote(newNote) =>
      ReasonReact.Update({...state, currentNote: Some(newNote)})
    | AddNote(newNote) =>
      ReasonReact.Update({
        ...state,
        notes: List.append(state.notes, [newNote]),
      })
    | SwitchTeam(newTeam) =>
      let teamPatients = getTeamPatients(state.patients, newTeam);
      let newPatient = getCurrentPatient(teamPatients);
      let patientNotes = getPatientNotes(state.notes, newPatient);
      let newNote = getCurrentNote(patientNotes);
      ReasonReact.Update({
        ...state,
        currentPatient: newPatient,
        currentTeam: newTeam,
        currentNote: newNote,
      });
    },
  render: self =>
    /* <DataLoader.ReasonContext.Provider value=(Some(storage))> */
    <div className="noteAssembler">
      <Storage.Context.Provider value=storage>
        <div className="mainTitle">
          (ReasonReact.string("NoteAssembler"))
        </div>
        <TeamSelector
          teams=self.state.teams
          currentTeam=self.state.currentTeam
          switchTeam=(newTeam => self.send(SwitchTeam(newTeam)))
        />
        <PatientsList
          teamTitle=self.state.currentTeam.title
          teamPatients=(
            getTeamPatients(self.state.patients, self.state.currentTeam)
          )
          switchPatient=(newPatient => self.send(SwitchPatient(newPatient)))
          removePatient=(oldPatient => self.send(RemovePatient(oldPatient)))
          addPatient=(newPatient => self.send(AddPatient(newPatient)))
        />
        <CurrentInfo currentPatient=self.state.currentPatient />
        <Subjective />
        <Objective
          patientNotecrafts=(
            getPatientNotecrafts(
              self.state.notecrafts,
              self.state.currentPatient,
            )
          )
          patientExams=(
            getPatientExams(self.state.exams, self.state.currentPatient)
          )
        />
        <Assessment
          currentPatient=self.state.currentPatient
          user=self.state.currentUser
          patientComments=(
            getPatientComments(self.state.comments, self.state.currentPatient)
          )
          addComment=(newComment => self.send(AddComment(newComment)))
          addNote=(newNote => self.send(AddNote(newNote)))
          patientNotes=(
            getPatientNotes(self.state.notes, self.state.currentPatient)
          )
          currentNote=self.state.currentNote
          switchNote=(newNote => self.send(SwitchNote(newNote)))
        />
        <GenerateButtons />
      </Storage.Context.Provider>
    </div>,
  /* </DataLoader.ReasonContext.Provider>, */
};

/* let componentsToDisplay: array(string) = [|"CommentsBox", "NoteBox"|];

   for (i in 0 to Array.length(componentsToDisplay)) {
     let component = componentsToDisplay[i];
     ReactDOMRe.renderToElementWithClassName(<component />, component);
   }; */

/* Js.Array.forEach(componentsToDisplay, comp =>
     ReactDOMRe.renderToElementWithClassName(<comp />, comp)
   ); */
