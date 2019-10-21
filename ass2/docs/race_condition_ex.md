```
1	username2
3	username3
```

- Thread_1
  - Gets user with username1
  - Sets id = 1
  - writes to the file
- Thread_2
  - Gets user with username2
  - Sets id = 1 (because it is still not incremented )
  - Overwrites username1 with username2 in id 1 (Race condition)
  - Increaments curr_id variable
- Thread_1
  - Increments curr_id variable
- Threads_3
  - receives username3 and curr_id 3, so everything good and it do its job done 