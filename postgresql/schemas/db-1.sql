DROP SCHEMA IF EXISTS ratings_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS ratings_schema;

CREATE TABLE IF NOT EXISTS ratings_schema.reviews (
    username TEXT,
    game TEXT,
    rating INTEGER,
    review TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY(username, game)
);

CREATE TABLE IF NOT EXISTS ratings_schema.users(
    name TEXT PRIMARY KEY,
    count INTEGER DEFAULT(1)
)
