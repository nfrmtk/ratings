import pytest

from testsuite.databases import pgsql

# Start the tests via `make test-debug` or `make test-release`


form = {
    'email': 'vasya@mail.ru',
    'password': 'vasya123',
    'username': 'bigboy300',
}


async def register(service_client):
    return await service_client.post(
        '/v1/register',
        form=form
    )


async def test_register(service_client):
    response = await register(service_client)
    assert response.status == 200


async def test_login(service_client):
    await register(service_client)
    response = await service_client.post(
        '/v1/login',
        json={
            'email': form['email'],
            'password': form['password']
        }
    )
    assert response.status == 200
    assert 'ticket' in response.json()

header = {
    'X-Ratings-User-Ticket': '40e6215d-b5c6-4896-987c-f30f3678f608'
}


@pytest.mark.pgsql('db_1', files=['initial_data_signed.sql'])
async def test_post_review(service_client):
    response = await service_client.post(
        '/v1/review',
        json={
            'email': 'vasya@mail.ru',
            'game': 'gta5',
            'rating': 10,
            "text": 'vkusnyatina'
        },
        headers=header
    )
    assert response.status == 200
    assert response.text[0:10:1] != '1970-01-01'


@pytest.mark.pgsql('db_1', files=['initial_data_signed.sql'])
async def test_get_all_reviews(service_client):
    response = await service_client.get(
        '/v1/reviews',
        params={},
        headers=header
    )
    assert response.status == 200
    assert len(response.json()) > 0


@pytest.mark.pgsql('db_1', files=['initial_data_signed.sql'])
async def test_post_already_in_db(service_client):
    data = {
        'username': 'grebnev2003',
        'game': 'gta',
        'rating': 1,
        "text": 'net na telefone'
    }
    response = await service_client.post(
        '/v1/review',
        json=data,
        headers=header
    )
    assert response.status == 200
    response = await service_client.post(
        '/v1/review',
        json=data,
        headers=header
    )
    assert response.status == 200
    assert response.text[0:10:1] == '1970-01-01'


@pytest.mark.pgsql('db_1', files=['initial_data_signed_with_reviews.sql'])
async def test_update(service_client):
    patch = {
        'username': 'vasya@mail.ru',
        'game': 'gta',
        'rating': 1,
        'text': 'net na telefone'
    }
    await service_client.patch(
        '/v1/review',
        json=patch
    )
    response = await service_client.get(
        '/v1/reviews',
        params={
            'game': 'gta',
            'username': 'vasya@mail.ru'
        }
    )
    assert len(response.json()) == 1
    assert response.json()[0] == patch
