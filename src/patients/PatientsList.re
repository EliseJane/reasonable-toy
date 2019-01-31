open Models;

[@bs.module] external uuidv4 : unit => string = "uuid/v4";
type action =
  | ToggleNew
  | ToggleConfirm
  | ChangeDeletingPatient(patient);
type state = {
  newOpen: bool,
  confirmOpen: bool,
  deletingPatient: option(patient),
};
let component = ReasonReact.reducerComponent("PatientsList");

let newPatient = (~name) => {
  pid: uuidv4(),
  names: [name],
  addresses: [{city: "San Francisco", state: "CA"}],
};

let make =
    (
      ~teamPatients,
      ~teamTitle,
      ~switchPatient,
      ~removePatient,
      ~addPatient,
      _children,
    ) => {
  ...component,
  initialState: () => {
    newOpen: false,
    confirmOpen: false,
    deletingPatient: None,
  },
  reducer: (action, state) =>
    switch (action) {
    | ToggleNew => ReasonReact.Update({...state, newOpen: ! state.newOpen})
    | ToggleConfirm =>
      ReasonReact.Update({...state, confirmOpen: ! state.confirmOpen})
    | ChangeDeletingPatient(patient) =>
      ReasonReact.Update({...state, deletingPatient: Some(patient)})
    },
  render: self =>
    <Storage.Context.Consumer>
      ...(
           storage =>
             <DataLoader
               dataStore=storage
               query=(Some("Patient/f2831186-85c1-42af-aab8-61d7c715d57a"))>
               <div className="patientsList">
                 <div> (ReasonReact.string(teamTitle)) </div>
                 <div className="patients">
                   (
                     teamPatients
                     |> List.map(patient =>
                          <div key=patient.pid>
                            <span
                              className="delete clickable"
                              onClick=(
                                _evt => {
                                  self.send(ChangeDeletingPatient(patient));
                                  self.send(ToggleConfirm);
                                }
                              )>
                              (ReasonReact.string("X"))
                            </span>
                            <div
                              className="patient clickable"
                              onClick=(_evt => switchPatient(patient))>
                              (
                                ReasonReact.string(
                                  List.hd(patient.names).family,
                                )
                              )
                            </div>
                          </div>
                        )
                     |> Array.of_list
                     |> ReasonReact.array
                   )
                 </div>
                 <AddNew onToggle=(_event => self.send(ToggleNew)) />
                 (
                   self.state.newOpen ?
                     <NewPatient
                       onSave=(
                         (name: string) => {
                           self.send(ToggleNew);
                           addPatient(
                             newPatient({
                               given: Array.of_list([name]),
                               family: name,
                               text: None,
                             }),
                           );
                         }
                       )
                       onCancel=(() => self.send(ToggleNew))
                     /> :
                     ReasonReact.null
                 )
                 (
                   self.state.confirmOpen ?
                     <AreYouSure
                       message=(
                         "Are you sure you want to remove patient "
                         ++ List.hd(
                              Js.Option.getExn(self.state.deletingPatient).
                                names,
                            ).
                              family
                         ++ " from "
                         ++ teamTitle
                         ++ "?"
                       )
                       onYes=(
                         _evt => {
                           removePatient(
                             Js.Option.getExn(self.state.deletingPatient),
                           );
                           self.send(ToggleConfirm);
                         }
                       )
                       onNo=(_evt => self.send(ToggleConfirm))
                     /> :
                     ReasonReact.null
                 )
               </div>
             </DataLoader>
         )
    </Storage.Context.Consumer>,
};
