module MainConfig = [%graphql
  {|
  query {
    siteStatistics {
      ...SiteStatistics.Fragment.Statistics
    }
  }
|}
];

[@react.component]
let make = () => {
  let (simple, _) = ApolloHooks.useQuery((module MainConfig));

  <div className="main-panel">
    <div className="content-wrapper">
      <Header />
      {switch (simple) {
       | Error(_) => <div> {"Ooops! En error occured." |> React.string} </div>
       | Data(data) => <SiteStatistics statistics=data##siteStatistics />
       | _ => React.null
       }}
      <div className="row">
        <div className="col-7 grid-margin"> <RecentTickets /> </div>
        <div className="col-5 grid-margin"> <TodoList /> </div>
      </div>
    </div>
  </div>;
};
