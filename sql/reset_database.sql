DELETE FROM Attachments;
DELETE FROM Tags_tree;
DELETE FROM Tags_of_events;
DELETE FROM Events;
DELETE FROM Tags_list;

INSERT INTO Tags_list VALUES (null, '-');
INSERT INTO Tags_tree SELECT Id, null FROM Tags_list WHERE Tag = '-';
