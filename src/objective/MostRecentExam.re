open Models;

let component = ReasonReact.statelessComponent("MostRecentExam");
let make = (~patientExams: list(exam), _children) => {
  ...component,
  render: _self =>
    List.length(patientExams) == 0 ?
      <div> (ReasonReact.string("No Exams")) </div> :
      <div className="exam">
        (ReasonReact.string("Most Recent Exam: "))
        <div> (ReasonReact.string(List.hd(patientExams).value)) </div>
      </div>,
};
