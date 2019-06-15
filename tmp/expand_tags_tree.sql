
WITH RECURSIVE nodes(tag_id) AS (
	SELECT Tags_tree.id
	FROM Tags_tree
	WHERE Tags_tree.id IN (
		SELECT id FROM Tags_list WHERE Tags_list.tag = 'b'
	)
	
	UNION ALL
	
	SELECT Tags_tree.id
	FROM Tags_tree
	JOIN nodes
	ON Tags_tree.parent_id = nodes.tag_id
)

SELECT Tags_list.tag
FROM Tags_list
JOIN (SELECT DISTINCT tag_id FROM nodes) tmp_table
ON tmp_table.tag_id = Tags_list.id;
