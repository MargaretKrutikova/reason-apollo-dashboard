## The whole app

`git checkout master`

Outline:

1. A simple todo list - a query to fetch them all
2. A simple mutation to add a todo + a way to refetch all todos with the newly added entity
3. Mutations to update and delete a todo
4. Pagination on tickets

## Simple todo list - a query to fetch them all

Let's zoom into just the part with todos and see how we can fetch a list of todos by defining a Todo fragment (to collocate data with the component that is going to render it) close to the Todo component and using `useQuery` hook.

`git checkout fetch-todos`

- Todo component: defines its fragment on TodoItem, uses @bsRecord to convert to record + type todo in the scope
- compile time error if incorrect or missing field name in fragment
- TodoList with useQuery, query status
- query is called immediately when the component renders, the result is kept in cache
- pattern match on query result in jsx
- access root query data with ##, compile error if incorrect name
- apollo dev tools with cache!

## Mutation to add todos

Let's see how we can add a todo using `useMutation` hook.

`git checkout add-todo`

- write mutation that accepts variables (strongly typed)
- mutation name is defined in the API (compile error if incorrect)
- difference from `useQuery`: does not automatically send the API request when the component renders
- returns a function that will perform the actual mutation, that can be called in an event handler, and the status of the mutation (variant with states similar to query result)
- difference from relay hooks - keeps track of the mutation status so that you can access the loading state to disable parts of the UI
- mutation takes in variables
- variables are strongly typed with graphql ppx
- variables can be created with `make` that will stringify the query and create object with variables
- `refetchQueries` to get the latest todos - a simpler solution than updating cache / optimistic response

## Mutation to update and delete todos

`git checkout update-todo`

- update mutation - toggle completed property on the todo using the same fragment as the todo item fragment
- why does the UI update when the completed property changes?
- if a mutation updates a single existing entity and returns id (+ all the other fields), Apollo will automatically update it in cache and update the UI depending on this property
- check apollo dev tools to see how stuff is updated in cache
- in case of creating/deleting entities you will need to manually update your cache, or provide `refetchQueries` when calling your mutation
- delete todo: need to refetch the todo list

## Pagination for recent tickets

`git checkout pagination`

- numbered pages as opposed to cursor based
- query with a variable
- call the same query, but with different variables, and concat prev items with the new items
- use apollo's fetchMore
- merging new paging result with the previous one from apollo cache
- since the data in cache is plain JS objects, we need to make sure that we are writing back JS objects and not records when merging the prev list with the new result, the shape has to be the same!
- network status gives us more fine-grained information about the current status - not just loading or data, but also whether a fetch more request is in progress.
- show apollo dev tools
