open Models;

let component = ReasonReact.statelessComponent("CurrentInfo");

let make = (~currentPatient, _children) => {
  ...component,
  render: _self =>
    switch (currentPatient) {
    | Some(currentPatient) =>
      <div className="currentInfo">
        (
          ReasonReact.string(
            Array.of_list(currentPatient.names)[0].family
            ++ " @ "
            ++ Array.of_list(currentPatient.addresses)[0].city
            ++ ", "
            ++ Array.of_list(currentPatient.addresses)[0].state,
          )
        )
      </div>
    | None => <div className="currentInfo" />
    },
};
