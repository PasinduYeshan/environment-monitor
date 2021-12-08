create database emonitor;
use emonitor;

create table sensor_data(
	id int primary key auto_increment,
	alert_id varchar(200) not null,
	time_stamp datetime not null,
	temperature float not null,
	t_sd float not null,
	humidity float not null,
	h_sd float not null,
	pressure float not null,
	p_sd float not null,
	light float not null,
	l_sd float not null
);