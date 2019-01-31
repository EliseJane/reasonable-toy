open Models;

module Context =
  ReasonReactContext.CreateContext({
    type state = option(dataStore);
    let name = "Storage";
    let defaultValue = None;
  });
