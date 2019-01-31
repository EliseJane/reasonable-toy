[%bs.raw {|require('../styles.css')|}];
open NoteAssembler;
open Models;

[@bs.deriving abstract]
type jsProps = {storage: dataStore};

let jsComponent =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(~storage=Some(jsProps |. storageGet), [||])
  );
