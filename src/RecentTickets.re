module TicketsConfig = [%graphql
  {|
  query($offset: Int!, $limit: Int!) {
    tickets(offset: $offset, limit: $limit) {
      __typename
      hasNextPage
      results {
        ...Ticket.Fragment.Ticket
      }
    }
  }
|}
];
module TicketsQuery = ReasonApolloHooks.Query.Make(TicketsConfig);

external toConfigType: Js.Json.t => TicketsConfig.t = "%identity";
external toJson: TicketsConfig.t => Js.Json.t = "%identity";

let mergeFetchMoreResult = (prevData, options): Js.Json.t => {
  let fetchMoreResult = options->ReasonApolloHooks.Query.fetchMoreResultGet;

  switch (fetchMoreResult) {
  | Some(currentData) =>
    let newResult = toConfigType(currentData)##tickets;
    let prevResult = toConfigType(prevData)##tickets;

    let mergedResult = {
      "tickets": {
        "__typename": newResult##__typename,
        "hasNextPage": newResult##hasNextPage,
        "results": prevResult##results->Belt.Array.concat(newResult##results),
      },
    };
    toJson(mergedResult);
  | None => prevData
  };
};

let limit = 2;

[@react.component]
let make = () => {
  let (_, full) =
    TicketsQuery.use(
      ~variables=TicketsConfig.make(~offset=0, ~limit, ())##variables,
      ~notifyOnNetworkStatusChange=true,
      (),
    );

  let isFetchingMore =
    full.networkStatus === ReasonApolloHooks.Types.FetchMore;

  let (hasNextPage, offset) =
    switch (full) {
    | {data: Some(data)} => (
        data##tickets##hasNextPage,
        data##tickets##results->Belt.Array.length,
      )
    | _ => (false, 0)
    };

  let handleLoadMore = () => {
    full.fetchMore(
      ~variables=TicketsConfig.make(~offset, ~limit, ())##variables,
      ~updateQuery=mergeFetchMoreResult,
      (),
    )
    |> ignore;
  };

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
          {switch (full) {
           | {loading: true, data: None} => React.null
           | {data: Some(data)} =>
             <tbody>
               {data##tickets##results
                ->Belt.Array.map(ticket => <Ticket key={ticket.id} ticket />)
                |> React.array}
             </tbody>
           | {error: Some(error)} =>
             <div> {ReasonReact.string(error##message)} </div>
           | {error: None, data: None, loading: false} =>
             React.string("Not asked")
           }}
        </table>
        {hasNextPage
           ? <button
               className="btn btn-gradient-primary font-weight-bold"
               id="add-task"
               onClick={_ => handleLoadMore()}
               disabled=isFetchingMore>
               {React.string("More")}
             </button>
           : React.null}
      </div>
    </div>
  </div>;
};
