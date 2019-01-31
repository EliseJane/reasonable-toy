open Models;

let component = ReasonReact.statelessComponent("NoteCraftExams");
let make = (~patientNotecrafts, _children) => {
  ...component,
  render: _self =>
    <div className="fromCraft">
      (ReasonReact.string("New from Note Craft: "))
      (
        patientNotecrafts
        |> List.map((craft: notecraft) =>
             <div key=craft.notecraftId>
               (ReasonReact.string(craft.category ++ ": " ++ craft.value))
             </div>
           )
        |> Array.of_list
        |> ReasonReact.array
      )
    </div>,
};
