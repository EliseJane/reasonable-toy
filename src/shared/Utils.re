let valueFromEvent = evt : string => (
                                       evt
                                       |> ReactEventRe.Form.target
                                       |> ReactDOMRe.domElementToObj
                                     )##value;

let extractId = refString : string => {
  let refLength = String.length(refString);
  let idIndex = String.index(refString, "/".[0]) + 1;
  let idLength = refLength - idIndex;
  String.sub(refString, idIndex, idLength);
};
