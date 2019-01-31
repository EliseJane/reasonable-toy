let component = ReasonReact.statelessComponent("Assessment");

let make =
    (
      ~currentPatient,
      ~user,
      ~patientComments,
      ~addComment,
      ~addNote,
      ~patientNotes,
      ~currentNote,
      ~switchNote,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="assessment">
      <div className="soapHeader"> (ReasonReact.string("Assessment")) </div>
      <CommentsBox patientComments currentPatient user addComment />
      <NoteSelector patientNotes switchNote />
      <NoteForm currentPatient currentNote user patientNotes addNote />
    </div>,
};
