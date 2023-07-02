import pytest

import aiohttp


from testsuite.databases import pgsql

# Start the tests via `make test-debug` or `make test-release`


form = {
    'email': 'vasya@mail.ru',
    'password': 'hG6x7RxcwqXZbDyC',
    'username': 'bigboy300',
}


def data_email_and_password(email, password):
    with aiohttp.MultipartWriter('form-data') as data:
        payload = aiohttp.payload.StringPayload(email)
        payload.set_content_disposition('form-data', name="email")
        data.append_payload(payload)
        payload = aiohttp.payload.StringPayload(password)
        payload.set_content_disposition('form-data', name="password")
        data.append_payload(payload)
    return data


def data_email_password_username(form):
    with aiohttp.MultipartWriter('form-data') as data:
        payload = aiohttp.payload.StringPayload(form['email'])
        payload.set_content_disposition('form-data', name="email")
        data.append_payload(payload)
        payload = aiohttp.payload.StringPayload(form['password'])
        payload.set_content_disposition('form-data', name="password")
        data.append_payload(payload)
        payload = aiohttp.payload.StringPayload(form['username'])
        payload.set_content_disposition('form-data', name="username")
        data.append_payload(payload)
    return data


async def test_register(service_client):
    good_form = form.copy()
    response = await service_client.post(
        '/v1/register',
        data=data_email_password_username(good_form)
    )
    assert response.status == 200


async def test_register_bad_password(service_client):
    bad_form = form.copy()
    bad_form['password'] = 'vasya'
    response = await service_client.post(
        '/v1/register',
        data=data_email_password_username(bad_form)
    )
    assert response.status == 400

async def test_register_bad_email(service_client):
    bad_form = form.copy()
    bad_form['email'] = 'vasya@mail.r'
    response = await service_client.post(
        '/v1/register',
        data=data_email_password_username(bad_form)
    )
    assert response.status == 400
async def test_login(service_client):
    good_form = form.copy()
    response = await service_client.post(
        '/v1/register',
        data=data_email_password_username(good_form)
    )
    assert response.status == 200
    response = await service_client.post(
        '/v1/login',
        data=data_email_and_password(form['email'], form['password'])
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
            'game': 'gta5',
            'rating': 10,
            "text": 'vkusnyatina'
        },
        headers=header
    )
    assert response.status == 200


@pytest.mark.pgsql('db_1', files=['initial_data_signed_with_reviews.sql'])
async def test_get_all_reviews(service_client):
    response = await service_client.get(
        '/v1/reviews',
        params={},
    )
    assert response.status == 200
    assert len(response.json()) > 0


@pytest.mark.pgsql('db_1', files=['initial_data_signed_with_reviews.sql'])
async def test_get_single_review(service_client):
    response = await service_client.get(
        '/v1/reviews',
        params={
            'game': 'gta',
            'email': 'vasya@mail.ru'
        },
    )
    assert response.status == 200
    assert response.json()[0]['text'] == 'nice game like it'


@pytest.mark.pgsql('db_1', files=['initial_data_signed_with_reviews.sql'])
async def test_post_already_in_db(service_client):
    data = {
        'game': 'gta',
        'rating': 1,
        "text": 'net na telefone'
    }
    response = await service_client.post(
        '/v1/review',
        json=data,
        headers=header
    )
    assert response.status == 409


@pytest.mark.pgsql('db_1', files=['initial_data_signed_with_reviews.sql'])
async def test_delete(service_client):
    response = await service_client.get(
        '/v1/reviews',
        params={
            'game': 'gta',
            'email': 'vasya@mail.ru'
        }
    )
    assert len(response.json()) == 1
    response = await service_client.delete(
        '/v1/review/gta',
        headers=header
    )
    assert response.status == 200
    response = await service_client.get(
        '/v1/reviews',
        params={
            'game': 'gta',
            'email': 'vasya@mail.ru'
        }
    )
    assert len(response.json()) == 0


@pytest.mark.pgsql('db_1', files=['initial_data_signed_with_reviews.sql'])
async def test_update(service_client):
    patch = {
        'game': 'gta',
        'rating': 1,
        'text': 'net na telefone'
    }
    response = await service_client.patch(
        '/v1/review',
        json=patch,
        headers=header
    )
    assert response.status == 200
    response = await service_client.get(
        '/v1/reviews',
        params={
            'game': 'gta',
            'email': 'vasya@mail.ru'
        }
    )
    assert len(response.json()) == 1
    assert response.json()[0]['text'] == patch['text']


@pytest.mark.pgsql('db_1', files=['initial_data_signed.sql'])
async def test_log_out(service_client):
    response = await service_client.post(
        '/v1/log-out',
        headers=header

    )
    assert response.status == 200

    response = await service_client.post(
        '/v1/review',
        json={
            'game': 'gta',
            'rating': 1,
            "text": 'net na telefone'
        },
        headers=header
    )
    assert response.status == 401
