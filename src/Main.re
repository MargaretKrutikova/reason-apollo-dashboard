[@react.component]
let make = () => {
  <div className="main-panel">
    <div className="content-wrapper">
      <Header />
      // <TopCardsDisplayer siteStatistics=query##siteStatistics />
      <div className="row">
        <div className="col-8 grid-margin"> <RecentTickets /> </div>
        <div className="col-4 grid-margin"> <TodoList /> </div>
      </div>
    </div>
  </div>;
};
