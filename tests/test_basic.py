import pytest

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`


async def test_first_time_users(service_client):
    response = await service_client.post(
        '/v1/hello',
        params={'name': 'userver'},
    )
    assert response.status == 200
    assert response.text == 'Hello, userver!\n'


async def test_db_updates(service_client):
    response = await service_client.post('/v1/hello', params={'name': 'World'})
    assert response.status == 200
    assert response.text == 'Hello, World!\n'

    response = await service_client.post('/v1/hello', params={'name': 'World'})
    assert response.status == 200
    assert response.text == 'Hi again, World!\n'

    response = await service_client.post('/v1/hello', params={'name': 'World'})
    assert response.status == 200
    assert response.text == 'Hi again, World!\n'


@pytest.mark.pgsql('db_1', files=['initial_data.sql'])
async def test_db_initial_data(service_client):
    response = await service_client.post(
        '/v1/hello',
        params={'name': 'user-from-initial_data.sql'},
    )
    assert response.status == 200
    assert response.text == 'Hi again, user-from-initial_data.sql!\n'


async def test_post_review(service_client):
    response = await service_client.post(
        '/v1/review',
        json={
            'username': 'vasya',
            'game': 'gta5',
            'rating': 10,
            "text": 'vkusnyatina'
        }
    )
    assert response.status == 200
    assert response.text[0:10:1] != '1970-01-01'


@pytest.mark.pgsql('db_1', files=['initial_data_reviews.sql'])
async def test_get_all_reviews(service_client):
    response = await service_client.get(
        '/v1/reviews',
        params={
        }
    )
    assert response.status == 200
    assert len(response.json()) == 4


@pytest.mark.pgsql('db_1', files=['initial_data_reviews.sql'])
async def test_post_already_in_db(service_client):
    response = await service_client.post(
        '/v1/review',
        json={
            'username': 'grebnev2003',
            'game': 'gta',
            'rating': 1,
            "text": 'net na telefone'
        }
    )
    assert response.status == 200
    assert response.text[0:10:1] == '1970-01-01'


@pytest.mark.pgsql('db_1', files=['initial_data_reviews.sql'])
async def test_update(service_client):
    body = {
        'username': 'grebnev2003',
        'game': 'gta',
        'rating': 1,
        'text': 'net na telefone'
    }
    await service_client.patch(
        '/v1/review',
        json=body
    )
    response = await service_client.get(
        '/v1/reviews',
        params={
            'game': 'gta',
            'username': 'grebnev2003'
        }
    )
    assert len(response.json()) == 1
    response = response.json()[0]
    assert response == body
