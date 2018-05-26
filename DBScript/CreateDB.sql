/*
数据库创建测试脚本
*/

create database if not exists mm_test default character set utf8;
use mm_test;

/*------------------------------*/
/*t_char						*/
/*------------------------------*/
drop table if exists t_char;
create table if not exists t_char
(
	aid				bigint					not null AUTO_INCREMENT,
	charguid		bigint					not null,
	accname			varchar(80) binary		not null default '',
	charname		varchar(80) binary		not null default '',
	hp				int						not null default -1,
	mp				int						not null default -1,
	title			blob					not null,
	isvalid			tinyint					not null default -1,
	primary key (aid)
)ENGINE = INNODB DEFAULT CHARSET=utf8;

create unique index Index_char_charguid on t_char
(
	charguid
);

create index Index_char_accname on t_char
(
	accname
);

create unique index Index_char_charname on t_char
(
	charname
);

delimiter //
drop procedure if exists createchar	//
create procedure createchar()
begin
end;//
delimiter ;

/*------------------------------*/
/*t_char_skill					*/
/*------------------------------*/
drop table if exists t_char_skill;
create table if not exists t_char_skill
(
	aid				bigint					not null AUTO_INCREMENT,
	charguid		bigint					not null,
	skillinfo		blob,
	isvalid			tinyint					not null default -1,
	primary key (aid)
)ENGINE = INNODB DEFAULT CHARSET=utf8;
