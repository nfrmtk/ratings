INSERT INTO
    ratings_schema.users
VALUES
    (
        'vasya@mail.ru',
        'bigboy300',
        'b4db9409712fb0a747b2bd8731bf3db7f1926d2136ab87933eb357668010d7b1'
    );

INSERT INTO
    ratings_schema.auth_sessions
VALUES
    (
        '40e6215d-b5c6-4896-987c-f30f3678f608',
        'vasya@mail.ru',
        DEFAULT
    );

INSERT INTO
    ratings_schema.reviews
VALUES
    ('vasya@mail.ru', 'gta', 10, 'nice game like it🥳');
