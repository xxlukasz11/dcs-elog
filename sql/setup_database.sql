CREATE TABLE Attachments(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	event_id INTEGER,
	name TEXT,
	type TEXT,
	file_name TEXT
);

CREATE TABLE Events(
	Id INTEGER PRIMARY KEY AUTOINCREMENT,
	Date INTEGER,
	Title TEXT,
	Description TEXT,
	author text
);

CREATE TABLE Tags_list(
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	tag TEXT
);

CREATE TABLE Tags_of_events(
	Event_id INTEGER NOT NULL,
	Tag_id INTEGER NOT NULL
);

CREATE TABLE Tags_tree(
	id INTEGER NOT NULL,
	parent_id INTEGER
);

INSERT INTO Tags_list VALUES (null, '-');
INSERT INTO Tags_tree SELECT id, null FROM Tags_list WHERE tag = '-';
