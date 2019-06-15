# version with id
# SELECT tmp.id AS id, tmp.tag AS tag, Tags_list.tag AS parent

SELECT
	tmp.tag AS tag,
	Tags_list.tag AS parent
FROM(
	SELECT Tags_list.id, Tags_list.tag, Tags_tree.parent_id
	FROM Tags_list
	JOIN Tags_tree
	ON Tags_tree.id = Tags_list.id
) tmp
LEFT JOIN Tags_list
ON Tags_list.id = tmp.parent_id