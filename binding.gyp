{
    'targets': [
        {
            'target_name': 'native_events',
            'sources': ['./src/channel.cc', './src/channels.cc', './src/native_events.cc'],
            'include_dirs' : ["<!(node -e \"require('nan')\")", "."]
        }
    ]
}
