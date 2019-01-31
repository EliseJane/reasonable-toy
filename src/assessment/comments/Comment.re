open Models;

let component = ReasonReact.statelessComponent("Comment");
let make = (~comment, ~onEdit, _children) => {
  ...component,
  render: _ =>
    <div>
      <span>
        (
          ReasonReact.string(
            Js.Date.toUTCString(comment.effectiveDateTime) ++ " ",
          )
        )
      </span>
      <span> (ReasonReact.string(comment.assessor ++ " entered ")) </span>
      <span onClick=(_evt => onEdit(comment.summary)) className="clickable">
        (ReasonReact.string("\"" ++ comment.summary ++ "\""))
      </span>
    </div>,
};
