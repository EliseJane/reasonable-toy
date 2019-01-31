let component = ReasonReact.statelessComponent("FormButtons");

let make = (~onSaveClick=?, ~onCancelClick=?, ~editorState=?, _children) => {
  ...component,
  render: _self =>
    <div className="formButtons">
      <button className="clickable" onClick=?onSaveClick>
        (ReasonReact.string("Save"))
      </button>
      <button className="clickable" onClick=?onCancelClick>
        (ReasonReact.string("Delete"))
      </button>
    </div>,
};
