// module QueryConfig = [%graphql {|
//   query {
//     tickets {

//     }
//   }
// |}];

[@react.component]
let make = () =>
  <div> {ReasonReact.string("hi")} <TodoList /> <RecentTickets /> </div>;
