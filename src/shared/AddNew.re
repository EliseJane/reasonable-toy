let component = ReasonReact.statelessComponent("AddNew");
let make = (~onToggle, _children) => {
  ...component,
  render: _ =>
    <a onClick=(_evt => onToggle()) className="clickable">
      (ReasonReact.string("Add new ..."))
    </a>,
};
