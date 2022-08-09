from django.urls import path

from . import views

app_name = 'files'
urlpatterns = [
    path('', views.index, name='index'),
    path('uploadFile', views.uploadFile, name='uploadFile'),
    path('confirm', views.confirm, name='confirm'),
]