delete from scorched3d_events where TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) > 14;
