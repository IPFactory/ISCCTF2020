app_path = '/app'

threads 256, 512
environment 'production'
preload_app!
bind 'tcp://0.0.0.0:4567/'
