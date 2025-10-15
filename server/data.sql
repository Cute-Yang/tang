# create table user
CREATE TABLE test_vote_user (
  # 用户id
  id INT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
  # 用户名
  user_name VARCHAR(50) UNIQUE NOT NULL,
  # 用户密码
  passwd VARCHAR(50) NOT NULL,
  # 用户邮箱,允许为空
  email VARCHAR(100),
  # 投票优先级
  vote_priority TINYINT UNSIGNED DEFAULT 1,
  # 用户的状态
  user_status TINYINT UNSIGNED  DEFAULT 0,
  # 创建时间
  create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  # 修改时间
  modify_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

# insert test datas
INSERT INTO test_vote_user (user_name, passwd, email, vote_priority, user_status) VALUES
('张伟', '123456', 'zhangwei@example.com', 1, 0),
('李娜', '123456', 'lina@example.com',  1, 0),
('王强', '123456', 'wangqiang@example.com',  1, 0),
('陈静', '123456', 'chenjing@example.com',  1, 0),
('刘洋', '123456', 'liuyang@example.com',  1, 0),
('赵敏', '123456', 'zhaomin@example.com',  1, 0),
('孙浩', '123456', 'sunhao@example.com',  1, 0),
('周琳', '123456', 'zhoulind@example.com',  1, 0),
('吴磊', '123456', 'wulei@example.ccreate_voteom',  1, 0),
('黄蓉', '123456', 'huangrong@example.com', 1, 0);

# create vote table
CREATE TABLE test_vote_data (
  # 自动生成的投票id
  vote_id INT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
  # 投票的主题,可以重复
  vote_topic VARCHAR(600) NOT NULL,
  # 投票类型/单选或者多选 0单选 1多选
  vote_type TINYINT NOT NULL,
  # 发起人
  vote_creator VARCHAR(50) NOT NULL,
  # 发起人的id,这个才是最关键的
  vote_creator_id INT UNSIGNED NOT NULL,
  # vote的状态(默认是ready)
  vote_status TINYINT UNSIGNED NOT NULL DEFAULT 0,
  # 创建时间
  vote_create_time TIMESTAMP NOT NULL
);
# insert test datas
INSERT INTO test_vote_data (vote_topic, vote_type, vote_creator,vote_creator_id ,vote_status,vote_create_time) VALUES
('公司年会去哪里举办？', 0, '周琳',8,0, NOW() - INTERVAL 15 DAY),
('本周团建活动选哪个？', 1, '黄蓉',10,0, NOW() - INTERVAL 14 DAY);

# 投票items
CREATE TABLE test_vote_item (
  id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
  vote_id INT UNSIGNED NOT NULL,
  vote_item VARCHAR(800) NOT NULL,
  vote_item_count INT UNSIGNED NOT NULL,
  vote_item_status TINYINT UNSIGNED NOT NULL DEFAULT 0,
  create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  -- 为 vote_id 添加索引（必须的）
  KEY idx_vote_id (vote_id),
  -- 可选：添加外键约束（推荐）
  CONSTRAINT fk_test_vote_item
    FOREIGN KEY (vote_id) REFERENCES test_vote_data(vote_id)
    ON DELETE CASCADE
    ON UPDATE NO ACTION
);

# vote item data
INSERT INTO test_vote_item (vote_id, vote_item,vote_count) VALUES
-- vote_id = 1: 公司年会去哪里举办？
(1, '三亚海边度假村',3),
(1, '北京古风民宿',2),
(1, '上海迪士尼乐园',1),
(1, '成都宽窄巷子文化游',5),

-- vote_id = 2: 本周团建活动选哪个？（多选）
(2, '密室逃脱',4),
(2, '轰趴馆游戏',3),
(2, '户外徒步登山',2),
(2, '桌游咖啡厅',7);


CREATE TABLE test_vote_participate_info (
  id BIGINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
  vote_id INT UNSIGNED NOT NULL,
  vote_status TINYINT UNSIGNED NOT NULL  DEFAULT 0,
--   //means whether processed!!!
  vote_process_status TINYINT UNSIGNED NOT NULL DEFAULT 0,
  voter_id INT UNSIGNED NOT NULL,
  voter_name VARCHAR(50) NOT NULL,
  create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  -- 为 vote_id 添加索引（必须的）
  KEY idx_vote_id (vote_id),
  -- 可选：添加外键约束（推荐）
  CONSTRAINT fk_test_vote_participate_item 
    FOREIGN KEY (vote_id) REFERENCES test_vote_data(vote_id)
    ON DELETE CASCADE
    ON UPDATE NO ACTION
);

INSERT INTO test_vote_participate_info (vote_id, voter_id,voter_name,create_time) VALUES
-- (1, 2,'李娜', '2025-10-01 08:30:00'),
-- (1, 3,'王强', '2025-10-01 09:15:00'),
-- (1, 8,'周琳', '2025-10-01 09:45:00'),
(1, 10,'黄蓉', '2025-10-01 09:45:00'),
-- 
-- (3, 5,'刘洋', '2025-10-01 08:30:00'),
-- (3, 4, '陈静','2025-10-01 09:15:00'),
-- (3, 9, '吴磊','2025-10-01 09:45:00'),
(3, 10,'黄蓉', '2025-10-03 09:45:00');
-- 
-- GRANT ALL PRIVILEGES ON vote.* TO 'sunny'@'%' IDENTIFIED BY 'aj4810SXV';
-- FLUSH PRIVILEGES;




DROP TABLE test_vote_participate_info;
