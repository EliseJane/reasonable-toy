let component = ReasonReact.statelessComponent("GenerateButtons");

let make = (~onSaveClick=?, ~onCancelClick=?, ~editorState=?, _children) => {
  ...component,
  render: _self =>
    <div className="generateButtons">
      <button className="clickable" onClick=?onSaveClick>
        (ReasonReact.string("Generate Clinical Note"))
      </button>
      <button className="clickable" onClick=?onCancelClick>
        (ReasonReact.string("Generate Billing Note"))
      </button>
    </div>,
};
