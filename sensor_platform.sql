create table node (
	id int not null primary key auto_increment,
	type int not null default 0 check(type between 0 and 2),
	idx int not null default 0,
	co2 int not null default 0,
	temperature double not null default 0.0,
	humidity double not null default 0.0,
	illumination double not null default 0.0,  
	create_time timestamp default now()
);
