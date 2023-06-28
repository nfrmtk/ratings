CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

DROP SCHEMA IF EXISTS ratings_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS ratings_schema;

CREATE TABLE IF NOT EXISTS ratings_schema.users(
    email TEXT PRIMARY KEY,
    username TEXT,
    password_hash TEXT
);

CREATE TABLE IF NOT EXISTS ratings_schema.reviews (
    email TEXT,
    game TEXT,
    rating int2,
    review TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY(email, game),
    FOREIGN KEY (email) references ratings_schema.users(email)
);

CREATE TABLE IF NOT EXISTS ratings_schema.auth_sessions(
    id TEXT PRIMARY KEY DEFAULT uuid_generate_v1(),
    email TEXT,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (email) REFERENCES ratings_schema.users(email)
);



