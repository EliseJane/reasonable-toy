open Utils;

type state = {commentText: string};
type action =
  | OnChange(string);
let component = ReasonReact.reducerComponent("EditComment");
let make = (~commentText="Enter your comment", ~onSave, ~onCancel, _children) => {
  ...component,
  initialState: () => {commentText: commentText},
  reducer: (action, _state) =>
    switch (action) {
    | OnChange(newText) => ReasonReact.Update({commentText: newText})
    },
  render: self =>
    <div>
      <textarea
        value=self.state.commentText
        onChange=(evt => self.send(OnChange(valueFromEvent(evt))))
      />
      <FormButtons
        onSaveClick=(_evt => onSave(self.state.commentText))
        onCancelClick=(_evt => onCancel())
      />
    </div>,
};
