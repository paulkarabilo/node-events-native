{
  'targets': [
    {
      'target_name': 'native_events',
      'sources': ['./src/native_events.cc', './src/channel.cc', './src/channels.cc'],
      'include_dirs' : ["<!(node -e \"require('nan')\")", "."]
    }
  ]
}