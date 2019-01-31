open Utils;

type state = {name: string};
type action =
  | OnChange(string);
let component = ReasonReact.reducerComponent("NewPatient");
let make = (~onSave, ~onCancel, _children) => {
  ...component,
  initialState: () => {name: ""},
  reducer: (action, _state) =>
    switch (action) {
    | OnChange(newText) => ReasonReact.Update({name: newText})
    },
  render: self =>
    <div>
      <input
        value=self.state.name
        onChange=(evt => self.send(OnChange(valueFromEvent(evt))))
      />
      <FormButtons
        onSaveClick=(_evt => onSave(self.state.name))
        onCancelClick=(_evt => onCancel())
      />
    </div>,
};
