## mysql
# Getid3PropsMap
CREATE TABLE DB_TABLE_PREFIXGetid3PropsMap(
 DB_COLUMN_PREFIXproperty varchar(128),
 DB_COLUMN_PREFIXviewMode int(11),
 DB_COLUMN_PREFIXsequence int(11),
 UNIQUE (DB_COLUMN_PREFIXproperty, DB_COLUMN_PREFIXviewMode)
) TYPE=DB_TABLE_TYPE;

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Getid3PropsMap', 1, 0);

## postgres
# Getid3PropsMap
CREATE TABLE DB_TABLE_PREFIXGetid3PropsMap(
 DB_COLUMN_PREFIXproperty VARCHAR(128),
 DB_COLUMN_PREFIXviewMode INTEGER,
 DB_COLUMN_PREFIXsequence INTEGER
);

CREATE UNIQUE INDEX DB_TABLE_PREFIXGetid3PropsMap_78674 ON DB_TABLE_PREFIXGetid3PropsMap(DB_COLUMN_PREFIXproperty, DB_COLUMN_PREFIXviewMode);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Getid3PropsMap', 1, 0);

## oracle
# Getid3PropsMap
CREATE TABLE DB_TABLE_PREFIXGetid3PropsMap(
 DB_COLUMN_PREFIXproperty VARCHAR2(128),
 DB_COLUMN_PREFIXviewMode INTEGER,
 DB_COLUMN_PREFIXsequence INTEGER
);

ALTER TABLE DB_TABLE_PREFIXGetid3PropsMap
 ADD UNIQUE (DB_COLUMN_PREFIXproperty, DB_COLUMN_PREFIXviewMode)
;

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Getid3PropsMap', 1, 0);

## db2
# Getid3PropsMap
CREATE TABLE DB_TABLE_PREFIXGetid3PropsMap(
 DB_COLUMN_PREFIXproperty VARCHAR(128),
 DB_COLUMN_PREFIXviewMode INTEGER,
 DB_COLUMN_PREFIXsequence INTEGER
);

CREATE UNIQUE INDEX DB_TABLE_PREFIXGetid02_78674  
 ON DB_TABLE_PREFIXGetid3PropsMap(DB_COLUMN_PREFIXproperty, DB_COLUMN_PREFIXviewMode);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Getid3PropsMap', 1, 0);

