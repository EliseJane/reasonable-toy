/* type loadedData;
   type subscription;
   type subscriber;
   [@bs.send]
   external subscribeFunc : ('a, subscriber) => subscription = "subscribe";
   type dataStore;
   [@bs.send] external queryFunc : (dataStore, string) => subscriber = "query";

   module ReasonContext =
     ReasonReactContext.CreateContext({
       type state = option(dataStore);
       let name = "reasonContext";
       let defaultValue: state = None;
     });

   module ReasonDataLoader = {
     type state = {
       data: option(loadedData),
       sub: option(subscription),
       query: string,
       dataStore,
     };

     type action =
       | UpdateData(loadedData)
       | UpdateSub(option(subscription));

     let rec unsubscribe = (s, selfSend) =>
       switch (s) {
       | Some(subscription) =>
         unsubscribe(s, selfSend);
         selfSend(UpdateSub(None));
       | None => ()
       };

     let subscribeToQuery = (dataStore: dataStore, query, selfSend) => {
       unsubscribe(None, selfSend);
       let newSub =
         queryFunc(dataStore, query)
         |> subscribeFunc(data => selfSend(UpdateData(data)));
       selfSend(UpdateSub(Some(newSub)));
       /* selfSend(
            UpdateSub(
              subscribeFunc((queryFunc(dataStore, query) =>
                selfSend(UpdateData(data)))
              ),
            ),
          ); */
     };

     let component = ReasonReact.reducerComponent("ReasonDataLoader");

     let make = (children, ~query, ~dataStore) => {
       ...component,
       initialState: () => {data: None, sub: None, query, dataStore},
       reducer: (action, state) =>
         switch (action) {
         | UpdateData(newData) =>
           ReasonReact.Update({...state, data: Some(newData)})
         | UpdateSub(newSub) => ReasonReact.Update({...state, sub: newSub})
         },
       willReceiveProps: self =>
         if (query != self.state.query || dataStore != self.state.dataStore) {
           {data: None, query, dataStore, sub: self.state.sub};
         } else {
           self.state;
         },
       didMount: self => subscribeToQuery(dataStore, query, self.send),
       didUpdate: ({oldSelf, newSelf}) =>
         if (newSelf.state.data == None) {
           subscribeToQuery(dataStore, query, newSelf.send);
         },
       willUnmount: self => unsubscribe(self.state.sub, self.send),
       render: self => <div />,
       /* switch (self.state.data) {
          | Some(loadedData) => Js.Option.getExn(children, self.state.data)
          | None => ReasonReact.null
          }, */
     };
   };

   let component = ReasonReact.statelessComponent("DataLoader");
   let make = (~query, children) => {
     ...component,
     render: _ =>
       <ReasonContext.Consumer>
         ...(dataStore => <ReasonDataLoader query ?dataStore />)
       </ReasonContext.Consumer>,
   }; */
