CREATE TABLE users (
    user_id INT IDENTITY(1,1) PRIMARY KEY,
    username VARCHAR(30) NOT NULL UNIQUE,
    CONSTRAINT CHK_username_length CHECK (LEN(username) BETWEEN 1 AND 30)
);

EXEC sp_rename 'users.user_id', 'id', 'COLUMN';

CREATE TABLE messages (
    id INT IDENTITY(1,1) PRIMARY KEY,
    sursa INT NOT NULL,
    destinatie INT NOT NULL,
    mesaj VARCHAR(4095) NOT NULL,
    FOREIGN KEY (sursa) REFERENCES users(id),
    FOREIGN KEY (destinatie) REFERENCES users(id)
);

drop table messages

-- Inserții pentru tabelul users
INSERT INTO users (username) VALUES
('john_doe'),
('jane_smith'),
('alex_123'),
('sara_mills');

-- Inserții pentru tabelul messages
INSERT INTO messages (sursa, destinatie, mesaj) VALUES
(1, 2, 'Bună Jane, cum merg lucrurile?'),
(2, 1, 'Salut John! Totul e bine, mulțumesc!'),
(3, 4, 'Hey Sara, ai auzit știrile de azi?'),
(4, 3, 'Nu încă, ce s-a întâmplat?');


SELECT username FROM (SELECT *, ROW_NUMBER() OVER(ORDER BY user_id) AS RowNum FROM users) AS UserWithRowNum WHERE RowNum >= 0 AND RowNum <=4;

