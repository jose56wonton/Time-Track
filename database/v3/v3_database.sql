-- MySQL Script generated by MySQL Workbench
-- Wed Jun 13 23:58:30 2018
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema timetrack
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema timetrack
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `timetrack` DEFAULT CHARACTER SET utf8 ;
USE `timetrack` ;

-- -----------------------------------------------------
-- Table `timetrack`.`authority`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`authority` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`authority` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `type` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `type_UNIQUE` (`type` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`crew`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`crew` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`crew` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`employee`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`employee` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`employee` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `first_name` VARCHAR(45) NOT NULL,
  `last_name` VARCHAR(45) NOT NULL,
  `pin` INT NOT NULL,
  `is_employed` TINYINT NOT NULL DEFAULT 1,
  `is_working` TINYINT NOT NULL,
  `authority_id` INT NOT NULL,
  `crew_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `pin_UNIQUE` (`pin` ASC),
  INDEX `fk_employee_authority1_idx` (`authority_id` ASC),
  INDEX `fk_employee_crew1_idx` (`crew_id` ASC),
  CONSTRAINT `fk_employee_authority1`
    FOREIGN KEY (`authority_id`)
    REFERENCES `timetrack`.`authority` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_employee_crew1`
    FOREIGN KEY (`crew_id`)
    REFERENCES `timetrack`.`crew` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`shift`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`shift` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`shift` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `clock_in_date` DATETIME NULL,
  `clock_out_date` DATETIME NULL,
  `length` INT NULL,
  `employee_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_shift_employee1_idx` (`employee_id` ASC),
  CONSTRAINT `fk_shift_employee1`
    FOREIGN KEY (`employee_id`)
    REFERENCES `timetrack`.`employee` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`category`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`category` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`category` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `type` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `type_UNIQUE` (`type` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`dimension`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`dimension` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`dimension` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `type` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `type_UNIQUE` (`type` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`subcategory`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`subcategory` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`subcategory` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `type` VARCHAR(45) NOT NULL,
  `category_id` INT NOT NULL,
  `dimension_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `type_UNIQUE` (`type` ASC),
  INDEX `fk_subcategory_category1_idx` (`category_id` ASC),
  INDEX `fk_subcategory_dimension1_idx` (`dimension_id` ASC),
  CONSTRAINT `fk_subcategory_category1`
    FOREIGN KEY (`category_id`)
    REFERENCES `timetrack`.`category` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_subcategory_dimension1`
    FOREIGN KEY (`dimension_id`)
    REFERENCES `timetrack`.`dimension` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`task`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`task` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`task` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  `is_active` TINYINT NOT NULL DEFAULT 1,
  `subcategory_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_task_subcategory1_idx` (`subcategory_id` ASC),
  CONSTRAINT `fk_task_subcategory1`
    FOREIGN KEY (`subcategory_id`)
    REFERENCES `timetrack`.`subcategory` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`project`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`project` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`project` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  `is_active` TINYINT NOT NULL DEFAULT 1,
  `date` DATETIME NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`project_task`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`project_task` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`project_task` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `task_id` INT NOT NULL,
  `project_id` INT NOT NULL,
  `quantity` INT NULL,
  `estimate_time` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_task_has_project_project1_idx` (`project_id` ASC),
  INDEX `fk_task_has_project_task1_idx` (`task_id` ASC),
  CONSTRAINT `fk_task_has_project_task1`
    FOREIGN KEY (`task_id`)
    REFERENCES `timetrack`.`task` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_task_has_project_project1`
    FOREIGN KEY (`project_id`)
    REFERENCES `timetrack`.`project` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `timetrack`.`activity`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `timetrack`.`activity` ;

CREATE TABLE IF NOT EXISTS `timetrack`.`activity` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `length` INT NOT NULL,
  `description` VARCHAR(45) NULL,
  `shift_id` INT NOT NULL,
  `project_task_id` INT NOT NULL,
  INDEX `fk_table1_shift1_idx` (`shift_id` ASC),
  PRIMARY KEY (`id`),
  INDEX `fk_activity_project_task1_idx` (`project_task_id` ASC),
  CONSTRAINT `fk_table1_shift1`
    FOREIGN KEY (`shift_id`)
    REFERENCES `timetrack`.`shift` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_activity_project_task1`
    FOREIGN KEY (`project_task_id`)
    REFERENCES `timetrack`.`project_task` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
