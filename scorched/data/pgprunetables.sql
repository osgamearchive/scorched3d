delete from scorched3d_events where NOW() - scorched3d_events.eventtime > interval '14 days';
