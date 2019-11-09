open Types;

let decodeDate = date => date->Belt.Option.flatMap(Js.Json.decodeString);

module Fragment = [%graphql
  {|
  fragment Ticket on Ticket @bsRecord {
    assignee {
      ... on User {
        ...Avatar.Fragment.User
      }
      ... on WorkingGroup @bsRecord {
        id
        name
      }
    }
    id
    subject
    trackingId
    status
    lastUpdated @bsDecoder(fn: "decodeDate")
  }
|}
];

[@react.component]
let make = (~ticket) => {
  <tr>
    <td>
      {switch (ticket.assignee) {
       | Some(assignee) =>
         switch (assignee) {
         | `User(user) => <Avatar user />
         | `WorkingGroup(workingGroup) =>
           <strong> {React.string(workingGroup.name)} </strong>
         }
       | None => <em> {React.string("Unassigned")} </em>
       }}
    </td>
    <td> {React.string(ticket.subject)} </td>
    <td> <TicketStatusBadge status={ticket.status} /> </td>
    <td>
      {React.string(Belt.Option.getWithDefault(ticket.lastUpdated, "-"))}
    </td>
    <td> {React.string(ticket.trackingId)} </td>
  </tr>;
};
