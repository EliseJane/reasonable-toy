open Models;

let fakeData = [%bs.raw {|require('../../FHIRdata.json')|}];

let nameDecoder = json =>
  Json.Decode.{
    family: json |> field("family", string),
    given: json |> field("given", array(string)),
    text: json |> optional(field("text", string)),
  };

let addressDecoder = json =>
  Json.Decode.{
    city: json |> field("city", string),
    state: json |> field("state", string),
  };

let patientDecoder = json =>
  Json.Decode.{
    pid: json |> field("id", string),
    names: json |> field("name", Json.Decode.list(nameDecoder)),
    addresses: json |> field("address", Json.Decode.list(addressDecoder)),
  };

let practitionerDecoder = json =>
  Json.Decode.{
    practitionerId: json |> field("id", string),
    practitionerNames: json |> field("name", Json.Decode.list(nameDecoder)),
  };

let itemDecoder = json =>
  Json.Decode.{ref: json |> field("reference", string)};

let entryDecoder = json =>
  Json.Decode.{item: json |> field("item", itemDecoder)};

let teamDecoder = json =>
  Json.Decode.{
    teamId: json |> field("id", string),
    title: json |> field("title", string),
    items: json |> field("entry", Json.Decode.list(entryDecoder)),
  };

let clinicalImpressionDecoder = json =>
  Json.Decode.{
    clinicalImpressionId: json |> field("id", string),
    description: json |> field("description", string),
    summary: json |> field("summary", string),
    dateTime: json |> field("date", date),
    assessor: json |> field("assessor", itemDecoder),
    subject: json |> field("subject", itemDecoder),
  };

let codeDecoder = json => Json.Decode.{code: json |> field("code", string)};

let codingDecoder = json =>
  Json.Decode.{
    coding: json |> field("coding", Json.Decode.list(codeDecoder)),
  };

let observationDecoder = json =>
  Json.Decode.{
    observationId: json |> field("id", string),
    category: json |> field("category", Json.Decode.list(codingDecoder)),
    code: json |> field("code", codingDecoder),
    subject: json |> field("subject", itemDecoder),
    value: json |> field("valueString", string),
  };

let dataDecoder = json =>
  Json.Decode.{
    patients: json |> field("patient", Json.Decode.list(patientDecoder)),
    practitioners:
      json |> field("practitioner", Json.Decode.list(practitionerDecoder)),
    teams: json |> field("list", Json.Decode.list(teamDecoder)),
    clinicalImpressions:
      json
      |> field(
           "clinicalImpression",
           Json.Decode.list(clinicalImpressionDecoder),
         ),
    observations:
      json |> field("observation", Json.Decode.list(observationDecoder)),
  };

let data: data = fakeData |> dataDecoder;
