open Models;

[@bs.module] external uuidv4 : unit => string = "uuid/v4";

type contentState;
[@bs.send] external getPlainText : contentState => string = "";
type editorState;
[@bs.send] external getCurrentContent : editorState => contentState = "";

[@bs.scope "EditorState"] [@bs.module "draft-js"]
external createEmpty : unit => editorState = "";
[@bs.scope "EditorState"] [@bs.module "draft-js"]
external createWithContent : editorState => editorState = "";
[@bs.scope "ContentState"] [@bs.module "draft-js"]
external createFromText : string => editorState = "";
[@bs.scope "RichUtils"] [@bs.module "draft-js"]
external handleKeyCommand : (editorState, string) => option(editorState) = "";

module Editor = {
  [@bs.module "draft-js"] external editor : ReasonReact.reactClass = "Editor";

  let make = (~editorState, ~onChange, ~handleKeyCommand, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=editor,
      ~props={
        "editorState": editorState,
        "onChange": onChange,
        "handleKeyCommand": handleKeyCommand,
      },
      children,
    );
};

type action =
  | ChangeState(editorState)
  | ToggleConfirm;
type state = {
  currentNote: option(note),
  editorState,
  confirmOpen: bool,
};
let component = ReasonReact.reducerComponent("NoteForm");

let makeEditorWithCurrentNote = (~currentNote: option(note)) => {
  let editorS =
    if (currentNote == None) {
      createEmpty();
    } else {
      let unwrappedNote = Js.Option.getExn(currentNote);
      let currentText = unwrappedNote.text;
      createWithContent(createFromText(currentText));
    };

  editorS;
};

let newNote = (~text, ~patient) => {
  let today: Js.Date.t = [%bs.raw {|new Date()|}];
  {noteId: uuidv4(), text, date: today, npid: patient.pid};
};

let make =
    (~user, ~currentPatient, ~currentNote, ~patientNotes, ~addNote, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | ChangeState(editorState) => ReasonReact.Update({...state, editorState})
    | ToggleConfirm =>
      ReasonReact.Update({...state, confirmOpen: ! state.confirmOpen})
    },
  initialState: () => {
    currentNote,
    editorState: makeEditorWithCurrentNote(currentNote),
    confirmOpen: false,
  },
  willReceiveProps: self =>
    if (self.state.currentNote != currentNote) {
      let newEditorState = makeEditorWithCurrentNote(currentNote);
      {...self.state, currentNote, editorState: newEditorState};
    } else {
      self.state;
    },
  render: self =>
    <div className="noteForm">
      <Editor
        editorState=self.state.editorState
        handleKeyCommand=(
          command => {
            let currentState = self.state.editorState;
            let newState = handleKeyCommand(currentState, command);
            switch (newState) {
            | None => "not-handled"
            | Some(editorState) =>
              self.send(ChangeState(Js.Option.getExn(newState)));
              "handled";
            };
          }
        )
        onChange=(editorState => self.send(ChangeState(editorState)))
      />
      <FormButtons
        editorState=self.state.editorState
        onCancelClick=(_evt => self.send(ToggleConfirm))
        onSaveClick=(
          _evt => {
            let editor = self.state.editorState;
            let content = getCurrentContent(editor);
            let text = getPlainText(content);
            addNote(newNote(text, Js.Option.getExn(currentPatient)));
          }
        )
      />
      (
        self.state.confirmOpen ?
          <AreYouSure
            onYes=(
              _evt => {
                self.send(ChangeState(createEmpty()));
                self.send(ToggleConfirm);
              }
            )
            onNo=(_evt => self.send(ToggleConfirm))
          /> :
          ReasonReact.null
      )
    </div>,
};

/* <button
     onClick=(
       _evt => {
         let currentState = self.state.editorState;
         Js.log(currentState);
         let newRich = [%bs.raw
           {|DraftJs.RichUtils.toggleInlineStyle(currentState, "BOLD")|}
         ];
         self.send(ChangeState(newRich));
       }
     )>
     (ReasonReact.string("Bold"))
   </button> */

/* let lastId = ref(4);
   let newNote = (~text, ~pid) => {
     lastId := lastId^ + 1;
     let today: Js.Date.t = [%bs.raw {|new Date()|}];
     {noteId: lastId^, text, date: today, pid};
   }; */
