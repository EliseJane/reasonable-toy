open Models;

[@bs.module] external uuidv4 : unit => string = "uuid/v4";

type state = {
  newOpen: bool,
  selectedCommentText: option(string),
};

type action =
  | ToggleNew
  | EditComment(string);

let newComment = (~summary, ~user, ~currentPatientVal) => {
  let today: Js.Date.t = [%bs.raw {|new Date()|}];
  {
    commentId: uuidv4(),
    summary,
    effectiveDateTime: today,
    assessor: user,
    cpid: currentPatientVal.pid,
  };
};

let component = ReasonReact.reducerComponent("CommentsBox");
let make = (~user, ~patientComments, ~currentPatient, ~addComment, _children) => {
  ...component,
  initialState: () => {newOpen: false, selectedCommentText: None},
  reducer: (action, state) =>
    switch (action) {
    | ToggleNew => ReasonReact.Update({...state, newOpen: ! state.newOpen})
    | EditComment(text) =>
      ReasonReact.Update({
        newOpen: ! state.newOpen,
        selectedCommentText: Some(text),
      })
    },
  render: self =>
    <div className="commentsBox">
      <div> (ReasonReact.string("Comments (click to edit)")) </div>
      <div className="comments">
        (
          patientComments
          |> List.map(comment =>
               <Comment
                 onEdit=(text => self.send(EditComment(text)))
                 comment
                 key=comment.commentId
               />
             )
          |> Array.of_list
          |> ReasonReact.array
        )
        <AddNew onToggle=(_event => self.send(ToggleNew)) />
      </div>
      (
        self.state.newOpen ?
          <EditComment
            commentText=?self.state.selectedCommentText
            onSave=(
              summary => {
                self.send(ToggleNew);
                let currentPatientVal = Js.Option.getExn(currentPatient);
                addComment(newComment(~summary, ~user, ~currentPatientVal));
              }
            )
            onCancel=(() => self.send(ToggleNew))
          /> :
          ReasonReact.null
      )
    </div>,
};
