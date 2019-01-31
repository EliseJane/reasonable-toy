open Models;
open Utils;

let component = ReasonReact.statelessComponent("TeamSelector");

let make = (~teams, ~switchTeam, ~currentTeam, _children) => {
  ...component,
  render: _self =>
    <div className="teamSelector">
      <select
        value=currentTeam.teamId
        onChange=(
          evt => {
            let newTeamId = valueFromEvent(evt);
            let newTeam = teams |> List.find(team => team.teamId == newTeamId);
            switchTeam(newTeam);
          }
        )>
        (
          teams
          |> List.map(team =>
               <option key=team.teamId value=team.teamId>
                 (ReasonReact.string(team.title))
               </option>
             )
          |> Array.of_list
          |> ReasonReact.array
        )
      </select>
    </div>,
};
