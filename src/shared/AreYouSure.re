let component = ReasonReact.statelessComponent("AreYouSure");
let make = (~onYes, ~onNo, ~message="Are you sure?", _children) => {
  ...component,
  render: _self =>
    <div className="confirmationModal">
      <span className="message"> (ReasonReact.string(message)) </span>
      <div className="buttons">
        <button onClick=onYes> (ReasonReact.string("Yes")) </button>
        <button onClick=onNo> (ReasonReact.string("No")) </button>
      </div>
    </div>,
};
