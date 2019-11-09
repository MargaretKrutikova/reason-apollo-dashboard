type todo = {
  id: string,
  text: string,
  completed: option(bool),
};

type user = {
  avatarUrl: option(string),
  fullName: string,
};

type workingGroup = {
  id: string,
  name: string,
};

type assignee = [ | `User(user) | `WorkingGroup(workingGroup)];

type status = [ | `Done | `Rejected | `OnHold | `Progress];

type ticket = {
  id: string,
  subject: string,
  trackingId: string,
  assignee: option(assignee),
  status,
  lastUpdated: option(string),
};

type siteStatistics = {
  weeklySales: float,
  weeklyOrders: int,
  currentVisitorsOnline: int,
};
