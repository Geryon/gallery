## mysql
# A_GalleryComment_1.0
ALTER TABLE DB_TABLE_PREFIXComment
  DROP INDEX DB_COLUMN_PREFIXdate,
  ADD INDEX DB_TABLE_PREFIXComment_95610(DB_COLUMN_PREFIXdate);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='Comment' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# GalleryComment
CREATE TABLE DB_TABLE_PREFIXComment(
 DB_COLUMN_PREFIXid int(11) NOT NULL,
 DB_COLUMN_PREFIXcommenterId int(11) NOT NULL,
 DB_COLUMN_PREFIXhost varchar(128) NOT NULL,
 DB_COLUMN_PREFIXsubject varchar(128),
 DB_COLUMN_PREFIXcomment text,
 DB_COLUMN_PREFIXdate int(11) NOT NULL,
 PRIMARY KEY(DB_COLUMN_PREFIXid),
 INDEX DB_TABLE_PREFIXComment_95610(DB_COLUMN_PREFIXdate)
) TYPE=DB_TABLE_TYPE;

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Comment', 1, 1);

## postgres
# A_GalleryComment_1.0
DROP INDEX DB_TABLE_PREFIXComment_95610;

CREATE INDEX DB_TABLE_PREFIXComment_95610 ON DB_TABLE_PREFIXComment(DB_COLUMN_PREFIXdate);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='Comment' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# GalleryComment
CREATE TABLE DB_TABLE_PREFIXComment(
 DB_COLUMN_PREFIXid INTEGER NOT NULL,
 DB_COLUMN_PREFIXcommenterId INTEGER NOT NULL,
 DB_COLUMN_PREFIXhost VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXsubject VARCHAR(128),
 DB_COLUMN_PREFIXcomment text,
 DB_COLUMN_PREFIXdate INTEGER NOT NULL
);

ALTER TABLE DB_TABLE_PREFIXComment ADD PRIMARY KEY (DB_COLUMN_PREFIXid);

CREATE INDEX DB_TABLE_PREFIXComment_95610 ON DB_TABLE_PREFIXComment(DB_COLUMN_PREFIXdate);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Comment', 1, 1);

## oracle
# A_GalleryComment_1.0
  DROP INDEX DB_TABLE_PREFIXComment_95610;

CREATE INDEX DB_TABLE_PREFIXComment_95610 ON DB_TABLE_PREFIXComment(DB_COLUMN_PREFIXdate);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='Comment' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# GalleryComment
CREATE TABLE DB_TABLE_PREFIXComment(
 DB_COLUMN_PREFIXid INTEGER NOT NULL,
 DB_COLUMN_PREFIXcommenterId INTEGER NOT NULL,
 DB_COLUMN_PREFIXhost VARCHAR2(128) NOT NULL,
 DB_COLUMN_PREFIXsubject VARCHAR2(128),
 DB_COLUMN_PREFIXcomment VARCHAR2(4000),
 DB_COLUMN_PREFIXdate INTEGER NOT NULL
);

CREATE INDEX DB_TABLE_PREFIXComment_95610
   ON DB_TABLE_PREFIXComment(DB_COLUMN_PREFIXdate);

ALTER TABLE DB_TABLE_PREFIXComment
 ADD PRIMARY KEY (DB_COLUMN_PREFIXid)
;

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Comment', 1, 1);

## db2
# A_GalleryComment_1.0
DROP INDEX DB_TABLE_PREFIXComme6d_95610;

CREATE INDEX DB_TABLE_PREFIXComme6d_95610 ON DB_TABLE_PREFIXComment(DB_COLUMN_PREFIXdate);

UPDATE DB_TABLE_PREFIXSchema
  SET DB_COLUMN_PREFIXmajor=1, DB_COLUMN_PREFIXminor=1
  WHERE DB_COLUMN_PREFIXname='Comment' AND DB_COLUMN_PREFIXmajor=1 AND DB_COLUMN_PREFIXminor=0;

# GalleryComment
CREATE TABLE DB_TABLE_PREFIXComment(
 DB_COLUMN_PREFIXid INTEGER NOT NULL,
 DB_COLUMN_PREFIXcommenterId INTEGER NOT NULL,
 DB_COLUMN_PREFIXhost VARCHAR(128) NOT NULL,
 DB_COLUMN_PREFIXsubject VARCHAR(128),
 DB_COLUMN_PREFIXcomment VARCHAR(8000),
 DB_COLUMN_PREFIXdate INTEGER NOT NULL
);

ALTER TABLE DB_TABLE_PREFIXComment ADD PRIMARY KEY (DB_COLUMN_PREFIXid);

CREATE INDEX DB_TABLE_PREFIXCommefc_95610
   ON DB_TABLE_PREFIXComment(DB_COLUMN_PREFIXdate);

INSERT INTO DB_TABLE_PREFIXSchema (
 DB_COLUMN_PREFIXname,
 DB_COLUMN_PREFIXmajor,
 DB_COLUMN_PREFIXminor
) VALUES('Comment', 1, 1);

