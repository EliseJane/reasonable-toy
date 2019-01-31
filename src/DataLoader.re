open Models;

type state = {
  data: option(datum),
  query: option(string),
  dataStore: option(dataStore),
};
type action =
  | UpdateData(datum)
  | UpdateSubscription(subscription);
let component = ReasonReact.reducerComponent("DataLoader");

let unSubScribe = () => ();

let subScribeToQuery = (dataStore, query, selfSend) => {
  unSubScribe();
  /* let whoKnowsWhat: subscriber = queryFunc(dataStore, query);
     let callback = data => {
       selfSend(UpdateData(data));
       data;
     };
     let subscription = subscribeFunc(whoKnowsWhat, callback: subscriber);
     selfSend(UpdateSubscription(subscription)); */
  /* selfSend(UpdateData(subscribeFunc(queryFunc(dataStore, query)))); */
  selfSend(UpdateData(queryFunc(query)));
  ();
  /* selfSend(
       UpdateSub(
         subscribeFunc((queryFunc(dataStore, query) =>
           selfSend(UpdateData(data)))
         ),
       ),
     ); */
};

let make = (~dataStore=?, ~query, ~children) => {
  ...component,
  initialState: () => {data: None, query: None, dataStore: None},
  reducer: (action, state) =>
    switch (action) {
    | UpdateData(data) => ReasonReact.Update({...state, data: Some(data)})
    },
  didMount: self =>
    subScribeToQuery(dataStore, Js.Option.getExn(query), self.send),
  didUpdate: ({oldSelf, newSelf}) =>
    if (oldSelf.state.data == None) {
      subScribeToQuery(dataStore, Js.Option.getExn(query), oldSelf.send);
    },
  willUnmount: _self => unSubScribe(),
  willReceiveProps: self =>
    if (query != self.state.query || dataStore != self.state.dataStore) {
      {data: None, query, dataStore};
    } else {
      self.state;
    },
  render: self =>
    switch (self.state.data) {
    | None => ReasonReact.null
    | Some(data) => children(data)
    },
};
