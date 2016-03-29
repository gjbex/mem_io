The database backend used by mem_io is [redis](http://redis.io/).  It is an open source in-memory data structure store.  mem_io store data, as well as metadata in the database.

The metadata is currently limited to the number of channels, stored using the key `<mem_io_id>:meta:nr_channels` as a string.

The data is stored in redis lists, one per channel ID.  To minimize the risk of data loss, `mem_io_store` typically performs multiple push operations.  The keys for the data have the following format: `<mem_io_id>:data:<channel_id>.  Here, `<channel_id>` is a zero-padded string representing the number of the channel.
