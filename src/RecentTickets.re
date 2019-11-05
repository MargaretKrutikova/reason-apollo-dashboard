module TicketsConfig = [%graphql
  {|
  query {
    tickets(offset: 0, limit: 20) {
      ...Ticket.Fragment.Ticket
    }
  }
|}
];

module TicketsQuery = ReasonApolloHooks.Query.Make(TicketsConfig);

[@react.component]
let make = () => {
  let (simple, _) = TicketsQuery.use();

  <div className="card">
    <div className="card-body">
      <h4 className="card-title"> {React.string("Recent Tickets")} </h4>
      <div className="table-responsive">
        <table className="table">
          <thead>
            <tr>
              <th> {React.string("Assignee")} </th>
              <th> {React.string("Subject")} </th>
              <th> {React.string("Status")} </th>
              <th> {React.string("Last Update")} </th>
              <th> {React.string("Tracking ID")} </th>
            </tr>
          </thead>
          <tbody>
            {switch (simple) {
             | Data(data) =>
               {data##tickets
                ->Belt.Array.map(todo
                    // TODO: make non-optional
                    =>
                      switch (todo) {
                      | Some(ticket) => <Ticket key={ticket.id} ticket />
                      | _ => React.null
                      }
                    )}
               |> React.array
             | _ => <div> {"Not clear" |> React.string} </div>
             }}
          </tbody>
        </table>
      </div>
    </div>
  </div>;
  // {hasNext
  //    ? <button
  //        className="btn btn-gradient-primary font-weight-bold"
  //        id="add-task"
  //        onClick={_ => loadNext(~count=2, ~onComplete=None) |> ignore}
  //        disabled=isLoadingNext>
  //        {React.string("More")}
  //      </button>
  //    : React.null}
};
