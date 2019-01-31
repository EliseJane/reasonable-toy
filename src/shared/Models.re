/* type subscription;
   type subscriber;
   type callback;
   [@bs.send] external subscribeFunc : callback => subscription = "subscribe";

   type promise; */
type datum;
type subscription;
type spoofStorage;
[@bs.deriving abstract]
type innerStorage = {spoofStorage};
type operation;
type operations = list(operation);
type subjects;
[@bs.deriving abstract]
type dataStore = {
  innerStorage,
  operations,
  subjects,
};
[@bs.send] external queryFunc : string => datum = "query";
/* [@bs.send] external queryFunc : string => (promise, subscriber) = "query"; */

type comment = {
  commentId: string,
  cpid: string,
  effectiveDateTime: Js.Date.t,
  assessor: string,
  summary: string,
};

type note = {
  noteId: string,
  npid: string,
  date: Js.Date.t,
  text: string,
};

type address = {
  city: string,
  state: string,
};

type name = {
  family: string,
  given: array(string),
  text: option(string),
};

type patient = {
  pid: string,
  names: list(name),
  addresses: list(address),
};

type practitioner = {
  practitionerId: string,
  practitionerNames: list(name),
};

type item = {ref: string};

type entry = {item};

type team = {
  teamId: string,
  title: string,
  items: list(entry),
};

type notecraft = {
  notecraftId: string,
  category: string,
  ncpid: string,
  value: string,
};

type exam = {
  examId: string,
  epid: string,
  value: string,
};

type models = {
  comments: list(comment),
  notes: list(note),
  patients: list(patient),
  practitioners: list(practitioner),
  teams: list(team),
  notecrafts: list(notecraft),
  exams: list(exam),
};

type clinicalImpression = {
  clinicalImpressionId: string,
  description: string,
  summary: string,
  dateTime: Js.Date.t,
  assessor: item,
  subject: item,
};

type code = {code: string};

type coding = {coding: list(code)};

type observation = {
  observationId: string,
  category: list(coding),
  code: coding,
  subject: item,
  value: string,
};

type data = {
  patients: list(patient),
  practitioners: list(practitioner),
  teams: list(team),
  clinicalImpressions: list(clinicalImpression),
  observations: list(observation),
};
