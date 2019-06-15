--single
SELECT
	Events.Id,
	datetime(Events.Date, 'unixepoch') AS Date,
	Events.Title AS Title,
	Events.Description AS Description,
	group_concat(Tags_list.Tag) AS Tags,
	Events.author AS Author
FROM (
	SELECT DISTINCT events.id
	FROM Events
	JOIN Tags_of_events ON Events.id = Tags_of_events.Event_id
	WHERE Tags_of_events.Tag_id = 1
) sub
JOIN Tags_of_events ON sub.Id = Tags_of_events.Event_id
JOIN Tags_list ON Tags_of_events.Tag_id = Tags_list.id
JOIN Events ON sub.Id = Events.Id
GROUP BY sub.Id;


--integrated with expanding tree of tags
SELECT
	Events.Id,
	datetime(Events.Date, 'unixepoch') AS Date,
	Events.Title AS Title,
	Events.Description AS Description,
	group_concat(Tags_list.Tag) AS Tags,
	Events.author AS Author
--ids of events that have desired tag and its children
FROM (
	SELECT DISTINCT events.id
	FROM Events
	--connect events with tags
	JOIN Tags_of_events ON Events.id = Tags_of_events.Event_id
	--local table that contains ids of desired tags and their children
	JOIN (
		WITH RECURSIVE nodes(tag_id) AS (
			--desired tags
			SELECT Tags_tree.id
			FROM Tags_tree
			--assign id to tag name
			WHERE Tags_tree.id IN (
				SELECT id
				FROM Tags_list
				WHERE Tags_list.tag = 'b' OR Tags_list.tag = 'c'
			)
			--union with children of selected tags
			UNION ALL
	
			SELECT Tags_tree.id
			FROM Tags_tree
			JOIN nodes
			ON Tags_tree.parent_id = nodes.tag_id
		)
		SELECT Tags_list.id AS id
		FROM Tags_list
		JOIN (SELECT DISTINCT tag_id FROM nodes) tmp_table
		ON tmp_table.tag_id = Tags_list.id
	) tags_ids
	ON Tags_of_events.Tag_id = tags_ids.id
) sub
--add ids of tags ovned by the selected events
JOIN Tags_of_events ON sub.Id = Tags_of_events.Event_id
--assign tags names to their ids
JOIN Tags_list ON Tags_of_events.Tag_id = Tags_list.id
--add rest of events table
JOIN Events ON sub.Id = Events.Id
-- specify date range
WHERE Events.Date >= strftime('%s', '2019-05-01') AND Events.Date <= strftime('%s', '2019-07-01')
--group for group_concat function
GROUP BY sub.Id
ORDER BY Events.Date DESC;