open Models;
let component = ReasonReact.statelessComponent("Objective");

let make =
    (
      ~patientNotecrafts: list(notecraft),
      ~patientExams: list(exam),
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="objective">
      <div className="soapHeader"> (ReasonReact.string("Objective")) </div>
      <div className="vitals">
        (ReasonReact.string("TODO: vital signs"))
      </div>
      <div>
        (ReasonReact.string("Physical Exam: "))
        <NoteCraftExams patientNotecrafts />
        <MostRecentExam patientExams />
      </div>
      <div className="labs">
        <div className="unreviewed">
          (ReasonReact.string("TODO: unreviewed labs"))
        </div>
        <div className="sticky">
          (ReasonReact.string("TODO: important labs"))
        </div>
      </div>
    </div>,
};
