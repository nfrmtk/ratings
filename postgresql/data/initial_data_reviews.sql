INSERT INTO ratings_schema.reviews(username, game, rating, review)
VALUES ('vasya', 'GTAV', '10', 'vkusnyatina'),
    ('fioletoviy', 'gtashka', '10', 'vsem igrat`'),
     ('grebnev2003', 'gta', '10', '' ),
    ('fiolet', 'gta5', '10', 'topchik!!!')
ON CONFLICT (username, game)
DO NOTHING;
