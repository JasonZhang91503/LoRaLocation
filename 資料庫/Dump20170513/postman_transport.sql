-- MySQL dump 10.13  Distrib 5.7.12, for Win64 (x86_64)
--
-- Host: localhost    Database: postman
-- ------------------------------------------------------
-- Server version	5.7.17-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `transport`
--

DROP TABLE IF EXISTS `transport`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `transport` (
  `_id` int(16) NOT NULL AUTO_INCREMENT,
  `requireTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `arriveTime` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `sender` int(16) NOT NULL,
  `receiver` int(16) NOT NULL,
  `start_id` int(16) NOT NULL,
  `des_id` int(16) NOT NULL,
  `car_id` int(16) NOT NULL,
  `state` int(2) NOT NULL DEFAULT '0',
  `key` varchar(4) NOT NULL,
  `remark` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`_id`),
  KEY `sender` (`sender`),
  KEY `receiver` (`receiver`),
  KEY `des_id` (`des_id`),
  KEY `car_id` (`car_id`),
  KEY `start_id` (`start_id`),
  CONSTRAINT `transport_ibfk_1` FOREIGN KEY (`sender`) REFERENCES `user` (`_id`),
  CONSTRAINT `transport_ibfk_2` FOREIGN KEY (`receiver`) REFERENCES `user` (`_id`),
  CONSTRAINT `transport_ibfk_3` FOREIGN KEY (`des_id`) REFERENCES `location` (`_id`),
  CONSTRAINT `transport_ibfk_4` FOREIGN KEY (`car_id`) REFERENCES `cars` (`_id`),
  CONSTRAINT `transport_ibfk_5` FOREIGN KEY (`start_id`) REFERENCES `location` (`_id`)
) ENGINE=InnoDB AUTO_INCREMENT=53 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-13 13:57:12
