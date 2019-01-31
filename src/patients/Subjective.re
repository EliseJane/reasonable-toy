let component = ReasonReact.statelessComponent("Subjective");

let make = _children => {
  ...component,
  render: _self =>
    <div className="subjective">
      <div className="soapHeader"> (ReasonReact.string("Subjective")) </div>
      <div className="text">
        (ReasonReact.string("subjective where does this come from?"))
      </div>
    </div>,
};
