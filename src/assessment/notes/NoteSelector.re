open Models;
open Utils;

let component = ReasonReact.statelessComponent("NotesList");

let make = (~patientNotes, ~switchNote, _children) => {
  ...component,
  render: _self =>
    /* <DataLoader query="Patient/f2831186-85c1-42af-aab8-61d7c715d57a">
       (
         data => {
           Js.log(data); */
    <div className="noteSelector">
      <select
        defaultValue="Select Note"
        onChange=(
          evt => {
            let newNoteId = valueFromEvent(evt);
            let newNote =
              patientNotes |> List.find(note => note.noteId == newNoteId);
            switchNote(newNote);
          }
        )>
        (
          patientNotes
          |> List.map(note =>
               <option key=note.noteId value=note.noteId>
                 (ReasonReact.string(Js.Date.toUTCString(note.date)))
               </option>
             )
          |> Array.of_list
          |> ReasonReact.array
        )
      </select>
    </div>,
  /* }
       )
     </DataLoader>, */
};
